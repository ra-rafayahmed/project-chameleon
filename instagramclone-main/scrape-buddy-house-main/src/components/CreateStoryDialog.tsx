import { useState } from 'react';
import { Dialog, DialogContent, DialogHeader, DialogTitle } from '@/components/ui/dialog';
import { Button } from '@/components/ui/button';
import { ImagePlus, X } from 'lucide-react';
import { useCurrentUser } from '@/hooks/useLocalStorage';
import { toast } from 'sonner';
import type { Story, StoryItem } from '@/data/mockData';

interface CreateStoryDialogProps {
  open: boolean;
  onOpenChange: (open: boolean) => void;
  onStoryCreated: (story: Story) => void;
}

export const CreateStoryDialog = ({ open, onOpenChange, onStoryCreated }: CreateStoryDialogProps) => {
  const { currentUser } = useCurrentUser();
  const [image, setImage] = useState<string>('');

  const handleImageUpload = (e: React.ChangeEvent<HTMLInputElement>) => {
    const file = e.target.files?.[0];
    if (file) {
      const reader = new FileReader();
      reader.onloadend = () => {
        setImage(reader.result as string);
      };
      reader.readAsDataURL(file);
    }
  };

  const handleCreate = () => {
    if (!image || !currentUser) return;

    const newStoryItem: StoryItem = {
      id: `story_item_${Date.now()}`,
      image: image,
      timestamp: 'Just now',
    };

    const newStory: Story = {
      id: `story_${currentUser.id}`,
      userId: currentUser.id,
      username: currentUser.username,
      avatar: currentUser.avatar,
      items: [newStoryItem],
      viewed: false,
    };

    onStoryCreated(newStory);
    setImage('');
    onOpenChange(false);
    toast.success('Story created successfully!');
  };

  return (
    <Dialog open={open} onOpenChange={onOpenChange}>
      <DialogContent className="max-w-md">
        <DialogHeader>
          <DialogTitle>Add to story</DialogTitle>
        </DialogHeader>

        <div className="space-y-4">
          {/* Image Upload */}
          {!image ? (
            <label className="flex flex-col items-center justify-center h-96 border-2 border-dashed border-border rounded-lg cursor-pointer hover:bg-muted/50 transition-colors">
              <ImagePlus className="h-12 w-12 text-muted-foreground mb-2" />
              <p className="text-sm text-muted-foreground">Click to upload story image</p>
              <input
                type="file"
                accept="image/*"
                className="hidden"
                onChange={handleImageUpload}
              />
            </label>
          ) : (
            <div className="relative">
              <img src={image} alt="Story preview" className="w-full h-96 object-cover rounded-lg" />
              <Button
                variant="destructive"
                size="icon"
                className="absolute top-2 right-2"
                onClick={() => setImage('')}
              >
                <X className="h-4 w-4" />
              </Button>
            </div>
          )}

          {/* Submit Button */}
          <Button
            onClick={handleCreate}
            disabled={!image}
            className="w-full"
          >
            Add to Story
          </Button>
        </div>
      </DialogContent>
    </Dialog>
  );
};
