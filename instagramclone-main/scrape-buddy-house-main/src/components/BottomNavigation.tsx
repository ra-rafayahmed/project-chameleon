import { Home, Search, PlusSquare, User } from 'lucide-react';
import { Link, useLocation } from 'react-router-dom';
import { getCurrentUser } from '@/lib/localStorage';

interface BottomNavigationProps {
  onCreatePost?: () => void;
}

export const BottomNavigation = ({ onCreatePost }: BottomNavigationProps) => {
  const location = useLocation();
  const currentUser = getCurrentUser();

  const isActive = (path: string) => location.pathname === path;

  return (
    <nav className="fixed bottom-0 left-0 right-0 bg-background border-t border-border z-50">
      <div className="max-w-2xl mx-auto flex items-center justify-around h-14 px-4">
        <Link to="/" className={`${isActive('/') ? 'text-foreground' : 'text-muted-foreground'} hover:text-foreground transition-colors`}>
          <Home className="h-6 w-6" />
        </Link>
        
        <Link to="/explore" className={`${isActive('/explore') ? 'text-foreground' : 'text-muted-foreground'} hover:text-foreground transition-colors`}>
          <Search className="h-6 w-6" />
        </Link>
        
        <button onClick={onCreatePost} className="text-muted-foreground hover:text-foreground transition-colors">
          <PlusSquare className="h-6 w-6" />
        </button>
        
        <Link to={`/profile/${currentUser}`} className={`${isActive(`/profile/${currentUser}`) ? 'text-foreground' : 'text-muted-foreground'} hover:text-foreground transition-colors`}>
          <User className="h-6 w-6" />
        </Link>
      </div>
    </nav>
  );
};
